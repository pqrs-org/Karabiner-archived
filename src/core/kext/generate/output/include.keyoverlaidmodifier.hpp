bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               KeyCode::VK_NONE, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           const KeyCode& fireKeyCode5,  const Flags& fireFlags5,
           bool isFireRepeat = false);

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
                     const KeyCode& fireKeyCode5,  const Flags& fireFlags5) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, fireFlags5,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           const KeyCode& fireKeyCode5,  const Flags& fireFlags5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, fireFlags5,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
                     const KeyCode& fireKeyCode5,  const Flags& fireFlags5) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, fireFlags5,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           const KeyCode& fireKeyCode5,  const Flags& fireFlags5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, fireFlags5,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
                     const KeyCode& fireKeyCode5,  const Flags& fireFlags5) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, fireFlags5,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           const KeyCode& fireKeyCode5,  const Flags& fireFlags5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, fireFlags5,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
                     const KeyCode& fireKeyCode5,  const Flags& fireFlags5) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, fireFlags5,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           const KeyCode& fireKeyCode5,  const Flags& fireFlags5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, fireFlags5,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
                     const KeyCode& fireKeyCode5,  const Flags& fireFlags5) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, fireFlags5,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           const KeyCode& fireKeyCode5,  const Flags& fireFlags5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, fireFlags5,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
                     const KeyCode& fireKeyCode5,  const Flags& fireFlags5) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, fireFlags5,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           const KeyCode& fireKeyCode5,  const Flags& fireFlags5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, fireFlags5,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
                     const KeyCode& fireKeyCode5,  const Flags& fireFlags5) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, fireFlags5,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           const KeyCode& fireKeyCode5,  const Flags& fireFlags5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, fireFlags5,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
                     const KeyCode& fireKeyCode5,  const Flags& fireFlags5) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, fireFlags5,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           const KeyCode& fireKeyCode5,  const Flags& fireFlags5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, fireFlags5,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
                     const KeyCode& fireKeyCode5,  const Flags& fireFlags5) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, fireFlags5,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           const KeyCode& fireKeyCode5,  const Flags& fireFlags5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, fireFlags5,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
                     const KeyCode& fireKeyCode5,  const Flags& fireFlags5) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, fireFlags5,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           const KeyCode& fireKeyCode5,  const Flags& fireFlags5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, fireFlags5,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
                     const KeyCode& fireKeyCode5,  const Flags& fireFlags5) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, fireFlags5,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           const KeyCode& fireKeyCode5,  const Flags& fireFlags5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, fireFlags5,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
                     const KeyCode& fireKeyCode5,  const Flags& fireFlags5) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, fireFlags5,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           const KeyCode& fireKeyCode5,  const Flags& fireFlags5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, fireFlags5,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
                     const KeyCode& fireKeyCode5,  const Flags& fireFlags5) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, fireFlags5,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           const KeyCode& fireKeyCode5,  const Flags& fireFlags5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, fireFlags5,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
                     const KeyCode& fireKeyCode5,  const Flags& fireFlags5) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, fireFlags5,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           const KeyCode& fireKeyCode5,  const Flags& fireFlags5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, fireFlags5,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
                     const KeyCode& fireKeyCode5,  const Flags& fireFlags5) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, fireFlags5,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           const KeyCode& fireKeyCode5,  const Flags& fireFlags5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, fireFlags5,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
                     const KeyCode& fireKeyCode5,  const Flags& fireFlags5) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, fireFlags5,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           const KeyCode& fireKeyCode5,  const Flags& fireFlags5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, fireFlags5,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
                     const KeyCode& fireKeyCode5,  const Flags& fireFlags5) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, fireFlags5,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           const KeyCode& fireKeyCode5,  const Flags& fireFlags5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, fireFlags5,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
                     const KeyCode& fireKeyCode5,  const Flags& fireFlags5) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, fireFlags5,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           const KeyCode& fireKeyCode5,  const Flags& fireFlags5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, fireFlags5,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
                     const KeyCode& fireKeyCode5,  const Flags& fireFlags5) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, fireFlags5,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           const KeyCode& fireKeyCode5,  const Flags& fireFlags5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, fireFlags5,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
                     const KeyCode& fireKeyCode5,  const Flags& fireFlags5) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, fireFlags5,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           const KeyCode& fireKeyCode5,  const Flags& fireFlags5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, fireFlags5,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
                     const KeyCode& fireKeyCode5,  const Flags& fireFlags5) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, fireFlags5,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           const KeyCode& fireKeyCode5,  const Flags& fireFlags5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, fireFlags5,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
                     const KeyCode& fireKeyCode5,  const Flags& fireFlags5) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, fireFlags5,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           const KeyCode& fireKeyCode5,  const Flags& fireFlags5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, fireFlags5,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
                     const KeyCode& fireKeyCode5,  const Flags& fireFlags5) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, fireFlags5,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           const KeyCode& fireKeyCode5,  const Flags& fireFlags5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, fireFlags5,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
                     const KeyCode& fireKeyCode5,  const Flags& fireFlags5) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, fireFlags5,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           const KeyCode& fireKeyCode5,  const Flags& fireFlags5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, fireFlags5,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
                     const KeyCode& fireKeyCode5,  const Flags& fireFlags5) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, fireFlags5,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           const KeyCode& fireKeyCode5,  const Flags& fireFlags5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, fireFlags5,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
                     const KeyCode& fireKeyCode5,  const Flags& fireFlags5) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, fireFlags5,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           const KeyCode& fireKeyCode5,  const Flags& fireFlags5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, fireFlags5,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
                     const KeyCode& fireKeyCode5,  const Flags& fireFlags5) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, fireFlags5,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           const KeyCode& fireKeyCode5,  const Flags& fireFlags5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, fireFlags5,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
                     const KeyCode& fireKeyCode5,  const Flags& fireFlags5) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, fireFlags5,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           const KeyCode& fireKeyCode5,  const Flags& fireFlags5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, fireFlags5,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
                     const KeyCode& fireKeyCode5,  const Flags& fireFlags5) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, fireFlags5,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           const KeyCode& fireKeyCode5,  const Flags& fireFlags5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, fireFlags5,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
                     const KeyCode& fireKeyCode5,  const Flags& fireFlags5) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, fireFlags5,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           const KeyCode& fireKeyCode5,  const Flags& fireFlags5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, fireFlags5,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
                     const KeyCode& fireKeyCode5,  const Flags& fireFlags5) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, fireFlags5,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           const KeyCode& fireKeyCode5,  const Flags& fireFlags5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, fireFlags5,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
                     const KeyCode& fireKeyCode5,  const Flags& fireFlags5) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, fireFlags5,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,
           const KeyCode& fireKeyCode5,  const Flags& fireFlags5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, fireFlags5,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4,
                     const KeyCode& fireKeyCode5,  const Flags& fireFlags5) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, fireFlags5,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,
           const KeyCode& fireKeyCode5,  const Flags& fireFlags5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, fireFlags5,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4,
                     const KeyCode& fireKeyCode5,  const Flags& fireFlags5) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, fireFlags5,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,
           const KeyCode& fireKeyCode5,  const Flags& fireFlags5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, fireFlags5,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4,
                     const KeyCode& fireKeyCode5,  const Flags& fireFlags5) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, fireFlags5,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,
           const KeyCode& fireKeyCode5,  const Flags& fireFlags5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, fireFlags5,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4,
                     const KeyCode& fireKeyCode5,  const Flags& fireFlags5) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, fireFlags5,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,
           const KeyCode& fireKeyCode5,  const Flags& fireFlags5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, fireFlags5,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4,
                     const KeyCode& fireKeyCode5,  const Flags& fireFlags5) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, fireFlags5,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,
           const KeyCode& fireKeyCode5,  const Flags& fireFlags5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, fireFlags5,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4,
                     const KeyCode& fireKeyCode5,  const Flags& fireFlags5) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, fireFlags5,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,
           const KeyCode& fireKeyCode5,  const Flags& fireFlags5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, fireFlags5,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4,
                     const KeyCode& fireKeyCode5,  const Flags& fireFlags5) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, fireFlags5,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,
           const KeyCode& fireKeyCode5,  const Flags& fireFlags5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, fireFlags5,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4,
                     const KeyCode& fireKeyCode5,  const Flags& fireFlags5) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, fireFlags5,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,
           const KeyCode& fireKeyCode5,  const Flags& fireFlags5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, fireFlags5,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4,
                     const KeyCode& fireKeyCode5,  const Flags& fireFlags5) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, fireFlags5,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,
           const KeyCode& fireKeyCode5,  const Flags& fireFlags5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, fireFlags5,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4,
                     const KeyCode& fireKeyCode5,  const Flags& fireFlags5) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, fireFlags5,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,
           const KeyCode& fireKeyCode5,  const Flags& fireFlags5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, fireFlags5,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4,
                     const KeyCode& fireKeyCode5,  const Flags& fireFlags5) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, fireFlags5,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,
           const KeyCode& fireKeyCode5,  const Flags& fireFlags5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, fireFlags5,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4,
                     const KeyCode& fireKeyCode5,  const Flags& fireFlags5) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, fireFlags5,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,
           const KeyCode& fireKeyCode5,  const Flags& fireFlags5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, fireFlags5,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4,
                     const KeyCode& fireKeyCode5,  const Flags& fireFlags5) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, fireFlags5,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,
           const KeyCode& fireKeyCode5,  const Flags& fireFlags5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, fireFlags5,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4,
                     const KeyCode& fireKeyCode5,  const Flags& fireFlags5) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, fireFlags5,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,
           const KeyCode& fireKeyCode5,  const Flags& fireFlags5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, fireFlags5,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4,
                     const KeyCode& fireKeyCode5,  const Flags& fireFlags5) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, fireFlags5,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,
           const KeyCode& fireKeyCode5,  const Flags& fireFlags5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, fireFlags5,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4,
                     const KeyCode& fireKeyCode5,  const Flags& fireFlags5) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, fireFlags5,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,
           const KeyCode& fireKeyCode5,  const Flags& fireFlags5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, fireFlags5,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4,
                     const KeyCode& fireKeyCode5,  const Flags& fireFlags5) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, fireFlags5,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,
           const KeyCode& fireKeyCode5,  const Flags& fireFlags5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, fireFlags5,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4,
                     const KeyCode& fireKeyCode5,  const Flags& fireFlags5) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, fireFlags5,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,
           const KeyCode& fireKeyCode5,  const Flags& fireFlags5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, fireFlags5,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4,
                     const KeyCode& fireKeyCode5,  const Flags& fireFlags5) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, fireFlags5,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,
           const KeyCode& fireKeyCode5,  const Flags& fireFlags5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, fireFlags5,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4,
                     const KeyCode& fireKeyCode5,  const Flags& fireFlags5) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, fireFlags5,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,
           const KeyCode& fireKeyCode5,  const Flags& fireFlags5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, fireFlags5,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4,
                     const KeyCode& fireKeyCode5,  const Flags& fireFlags5) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, fireFlags5,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,
           const KeyCode& fireKeyCode5,  const Flags& fireFlags5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, fireFlags5,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4,
                     const KeyCode& fireKeyCode5,  const Flags& fireFlags5) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, fireFlags5,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,
           const KeyCode& fireKeyCode5,  const Flags& fireFlags5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, fireFlags5,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4,
                     const KeyCode& fireKeyCode5,  const Flags& fireFlags5) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, fireFlags5,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,
           const KeyCode& fireKeyCode5,  const Flags& fireFlags5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, fireFlags5,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4,
                     const KeyCode& fireKeyCode5,  const Flags& fireFlags5) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, fireFlags5,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,
           const KeyCode& fireKeyCode5,  const Flags& fireFlags5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, fireFlags5,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4,
                     const KeyCode& fireKeyCode5,  const Flags& fireFlags5) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, fireFlags5,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,
           const KeyCode& fireKeyCode5,  const Flags& fireFlags5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, fireFlags5,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4,
                     const KeyCode& fireKeyCode5,  const Flags& fireFlags5) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, fireFlags5,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,
           const KeyCode& fireKeyCode5,  const Flags& fireFlags5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, fireFlags5,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4,
                     const KeyCode& fireKeyCode5,  const Flags& fireFlags5) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, fireFlags5,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,
           const KeyCode& fireKeyCode5,  const Flags& fireFlags5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, fireFlags5,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4,
                     const KeyCode& fireKeyCode5,  const Flags& fireFlags5) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, fireFlags5,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,
           const KeyCode& fireKeyCode5,  const Flags& fireFlags5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, fireFlags5,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4,
                     const KeyCode& fireKeyCode5,  const Flags& fireFlags5) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, fireFlags5,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,
           const KeyCode& fireKeyCode5,  const Flags& fireFlags5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, fireFlags5,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4,
                     const KeyCode& fireKeyCode5,  const Flags& fireFlags5) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, fireFlags5,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,
           const KeyCode& fireKeyCode5,  const Flags& fireFlags5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, fireFlags5,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4,
                     const KeyCode& fireKeyCode5,  const Flags& fireFlags5) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, fireFlags5,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,
           const KeyCode& fireKeyCode5,  const Flags& fireFlags5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, fireFlags5,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4,
                     const KeyCode& fireKeyCode5,  const Flags& fireFlags5) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, fireFlags5,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           const KeyCode& fireKeyCode5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
                     const KeyCode& fireKeyCode5) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           const KeyCode& fireKeyCode5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
                     const KeyCode& fireKeyCode5) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           const KeyCode& fireKeyCode5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
                     const KeyCode& fireKeyCode5) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           const KeyCode& fireKeyCode5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
                     const KeyCode& fireKeyCode5) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           const KeyCode& fireKeyCode5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
                     const KeyCode& fireKeyCode5) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           const KeyCode& fireKeyCode5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
                     const KeyCode& fireKeyCode5) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           const KeyCode& fireKeyCode5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
                     const KeyCode& fireKeyCode5) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           const KeyCode& fireKeyCode5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
                     const KeyCode& fireKeyCode5) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           const KeyCode& fireKeyCode5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
                     const KeyCode& fireKeyCode5) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           const KeyCode& fireKeyCode5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
                     const KeyCode& fireKeyCode5) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           const KeyCode& fireKeyCode5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
                     const KeyCode& fireKeyCode5) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           const KeyCode& fireKeyCode5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
                     const KeyCode& fireKeyCode5) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           const KeyCode& fireKeyCode5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
                     const KeyCode& fireKeyCode5) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           const KeyCode& fireKeyCode5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
                     const KeyCode& fireKeyCode5) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           const KeyCode& fireKeyCode5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
                     const KeyCode& fireKeyCode5) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           const KeyCode& fireKeyCode5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
                     const KeyCode& fireKeyCode5) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           const KeyCode& fireKeyCode5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
                     const KeyCode& fireKeyCode5) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           const KeyCode& fireKeyCode5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
                     const KeyCode& fireKeyCode5) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           const KeyCode& fireKeyCode5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
                     const KeyCode& fireKeyCode5) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           const KeyCode& fireKeyCode5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
                     const KeyCode& fireKeyCode5) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           const KeyCode& fireKeyCode5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
                     const KeyCode& fireKeyCode5) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           const KeyCode& fireKeyCode5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
                     const KeyCode& fireKeyCode5) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           const KeyCode& fireKeyCode5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
                     const KeyCode& fireKeyCode5) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           const KeyCode& fireKeyCode5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
                     const KeyCode& fireKeyCode5) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           const KeyCode& fireKeyCode5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
                     const KeyCode& fireKeyCode5) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           const KeyCode& fireKeyCode5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
                     const KeyCode& fireKeyCode5) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           const KeyCode& fireKeyCode5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
                     const KeyCode& fireKeyCode5) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           const KeyCode& fireKeyCode5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
                     const KeyCode& fireKeyCode5) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           const KeyCode& fireKeyCode5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
                     const KeyCode& fireKeyCode5) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           const KeyCode& fireKeyCode5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
                     const KeyCode& fireKeyCode5) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           const KeyCode& fireKeyCode5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
                     const KeyCode& fireKeyCode5) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
           const KeyCode& fireKeyCode5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4,  const Flags& fireFlags4,
                     const KeyCode& fireKeyCode5) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, fireFlags4,
               fireKeyCode5, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,
           const KeyCode& fireKeyCode5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4,
                     const KeyCode& fireKeyCode5) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,
           const KeyCode& fireKeyCode5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4,
                     const KeyCode& fireKeyCode5) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,
           const KeyCode& fireKeyCode5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4,
                     const KeyCode& fireKeyCode5) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,
           const KeyCode& fireKeyCode5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4,
                     const KeyCode& fireKeyCode5) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,
           const KeyCode& fireKeyCode5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4,
                     const KeyCode& fireKeyCode5) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,
           const KeyCode& fireKeyCode5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4,
                     const KeyCode& fireKeyCode5) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,
           const KeyCode& fireKeyCode5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4,
                     const KeyCode& fireKeyCode5) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,
           const KeyCode& fireKeyCode5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4,
                     const KeyCode& fireKeyCode5) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,
           const KeyCode& fireKeyCode5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4,
                     const KeyCode& fireKeyCode5) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,
           const KeyCode& fireKeyCode5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4,
                     const KeyCode& fireKeyCode5) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,
           const KeyCode& fireKeyCode5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4,
                     const KeyCode& fireKeyCode5) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,
           const KeyCode& fireKeyCode5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4,
                     const KeyCode& fireKeyCode5) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,
           const KeyCode& fireKeyCode5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4,
                     const KeyCode& fireKeyCode5) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,
           const KeyCode& fireKeyCode5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4,
                     const KeyCode& fireKeyCode5) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,
           const KeyCode& fireKeyCode5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4,
                     const KeyCode& fireKeyCode5) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
           const KeyCode& fireKeyCode4,
           const KeyCode& fireKeyCode5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,  const Flags& fireFlags3,
                     const KeyCode& fireKeyCode4,
                     const KeyCode& fireKeyCode5) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, fireFlags3,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,
           const KeyCode& fireKeyCode5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4,
                     const KeyCode& fireKeyCode5) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,
           const KeyCode& fireKeyCode5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4,
                     const KeyCode& fireKeyCode5) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,
           const KeyCode& fireKeyCode5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4,
                     const KeyCode& fireKeyCode5) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,
           const KeyCode& fireKeyCode5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4,
                     const KeyCode& fireKeyCode5) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,
           const KeyCode& fireKeyCode5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4,
                     const KeyCode& fireKeyCode5) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,
           const KeyCode& fireKeyCode5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4,
                     const KeyCode& fireKeyCode5) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,
           const KeyCode& fireKeyCode5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4,
                     const KeyCode& fireKeyCode5) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,
           const KeyCode& fireKeyCode5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,  const Flags& fireFlags2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4,
                     const KeyCode& fireKeyCode5) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, fireFlags2,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,
           const KeyCode& fireKeyCode5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4,
                     const KeyCode& fireKeyCode5) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,
           const KeyCode& fireKeyCode5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4,
                     const KeyCode& fireKeyCode5) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,
           const KeyCode& fireKeyCode5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4,
                     const KeyCode& fireKeyCode5) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,
           const KeyCode& fireKeyCode5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,  const Flags& fireFlags1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4,
                     const KeyCode& fireKeyCode5) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, fireFlags1,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,
           const KeyCode& fireKeyCode5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4,
                     const KeyCode& fireKeyCode5) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,     const Flags& toFlags,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,
           const KeyCode& fireKeyCode5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,     const Flags& toFlags,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4,
                     const KeyCode& fireKeyCode5) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, toFlags,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,   const Flags& fromFlags,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,
           const KeyCode& fireKeyCode5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,   const Flags& fromFlags,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4,
                     const KeyCode& fireKeyCode5) {
  return remap(remapParams,
               fromKeyCode, fromFlags,
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, ModifierFlag::NONE,
               true);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode,
           const KeyCode& fireKeyCode1,
           const KeyCode& fireKeyCode2,
           const KeyCode& fireKeyCode3,
           const KeyCode& fireKeyCode4,
           const KeyCode& fireKeyCode5,
           bool isFireRepeat = false) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, ModifierFlag::NONE,
               isFireRepeat);
}

bool remapWithRepeat(RemapParams& remapParams,
                     const KeyCode& fromKeyCode,
                     const KeyCode& toKeyCode,
                     const KeyCode& fireKeyCode1,
                     const KeyCode& fireKeyCode2,
                     const KeyCode& fireKeyCode3,
                     const KeyCode& fireKeyCode4,
                     const KeyCode& fireKeyCode5) {
  return remap(remapParams,
               fromKeyCode, Flags(0),
               toKeyCode, ModifierFlag::NONE,
               fireKeyCode1, ModifierFlag::NONE,
               fireKeyCode2, ModifierFlag::NONE,
               fireKeyCode3, ModifierFlag::NONE,
               fireKeyCode4, ModifierFlag::NONE,
               fireKeyCode5, ModifierFlag::NONE,
               true);
}

