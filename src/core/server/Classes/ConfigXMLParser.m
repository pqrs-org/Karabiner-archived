#import "ConfigXMLParser.h"
#import "bridge.h"

@implementation ConfigXMLParser

// ======================================================================
// filter

- (void) append_to_filter:(NSMutableArray*)filters node:(NSXMLNode*)node prefix:(NSString*)prefix filtertype:(unsigned int)filtertype
{
  NSArray* a = [[node stringValue] componentsSeparatedByString:@","];

  NSUInteger count = [a count] + 1;
  [filters addObject:[NSNumber numberWithUnsignedInteger:count]];
  [filters addObject:[NSNumber numberWithUnsignedInt:filtertype]];

  for (NSString* name in a) {
    [filters addObject:[NSNumber numberWithUnsignedInt:[keycode_ unsignedIntValue:[NSString stringWithFormat:@"%@%@", prefix, [KeyCode normalizeName:name]]]]];
  }
}

- (NSMutableArray*) make_filtervec:(NSXMLNode*)node
{
  NSMutableArray* filters = [NSMutableArray arrayWithCapacity:0];

  for (;;) {
    NSString* node_name = [node name];
    if ([node_name isEqualToString:@"item"]) break;
    if ([node_name isEqualToString:@"root"]) break;

    NSXMLNode* parent = [node parent];

    NSUInteger count = [parent childCount];
    for (NSUInteger i = 0; i < count; ++i) {
      NSXMLNode* n = [parent childAtIndex:i];
      if ([n kind] != NSXMLElementKind) continue;

      NSString* n_name = [n name];
      /*  */ if ([n_name isEqualToString:@"not"]) {
        [self append_to_filter:filters node:n prefix:@"ApplicationType::" filtertype:BRIDGE_FILTERTYPE_APPLICATION_NOT];
      } else if ([n_name isEqualToString:@"only"]) {
        [self append_to_filter:filters node:n prefix:@"ApplicationType::" filtertype:BRIDGE_FILTERTYPE_APPLICATION_ONLY];

      } else if ([n_name isEqualToString:@"device_not"]) {
        [self append_to_filter:filters node:n prefix:@"" filtertype:BRIDGE_FILTERTYPE_DEVICE_NOT];
      } else if ([n_name isEqualToString:@"device_only"]) {
        [self append_to_filter:filters node:n prefix:@"" filtertype:BRIDGE_FILTERTYPE_DEVICE_ONLY];

      } else if ([n_name isEqualToString:@"config_not"]) {
        [self append_to_filter:filters node:n prefix:@"ConfigIndex::" filtertype:BRIDGE_FILTERTYPE_CONFIG_NOT];
      } else if ([n_name isEqualToString:@"config_only"]) {
        [self append_to_filter:filters node:n prefix:@"ConfigIndex::" filtertype:BRIDGE_FILTERTYPE_CONFIG_ONLY];

      } else if ([n_name isEqualToString:@"inputmode_not"]) {
        [self append_to_filter:filters node:n prefix:@"InputMode::" filtertype:BRIDGE_FILTERTYPE_INPUTMODE_NOT];
      } else if ([n_name isEqualToString:@"inputmode_only"]) {
        [self append_to_filter:filters node:n prefix:@"InputMode::" filtertype:BRIDGE_FILTERTYPE_INPUTMODE_ONLY];

      } else if ([n_name isEqualToString:@"inputmodedetail_not"]) {
        [self append_to_filter:filters node:n prefix:@"InputModeDetail::" filtertype:BRIDGE_FILTERTYPE_INPUTMODEDETAIL_NOT];
      } else if ([n_name isEqualToString:@"inputmodedetail_only"]) {
        [self append_to_filter:filters node:n prefix:@"InputModeDetail::" filtertype:BRIDGE_FILTERTYPE_INPUTMODEDETAIL_ONLY];
      }
    }

    node = parent;
  }

  return filters;
}

// ======================================================================
// autogen

- (NSMutableArray*) combination:(NSArray*)input
{
  if ([input count] == 0) {
    NSMutableArray* a = [NSMutableArray arrayWithCapacity:0];
    return [NSMutableArray arrayWithObject:a];
  }

  id last = [input lastObject];

  NSRange range;
  range.location = 0;
  range.length = [input count] - 1;
  NSMutableArray* subarray = [self combination:[input subarrayWithRange:range]];

  NSMutableArray* newarray = [NSMutableArray arrayWithArray:subarray];
  for (NSMutableArray* a in subarray) {
    [newarray addObject:[a arrayByAddingObject:last]];
  }
  return newarray;
}

- (NSString*) getextrakey:(NSString*)keyname
{
  if ([keyname isEqualToString:@"HOME"])           { return @"CURSOR_LEFT";  }
  if ([keyname isEqualToString:@"END"])            { return @"CURSOR_RIGHT"; }
  if ([keyname isEqualToString:@"PAGEUP"])         { return @"CURSOR_UP";    }
  if ([keyname isEqualToString:@"PAGEDOWN"])       { return @"CURSOR_DOWN";  }
  if ([keyname isEqualToString:@"FORWARD_DELETE"]) { return @"DELETE";       }
  return @"";
}

- (void) append_to_initialize_vector:(NSMutableArray*)initialize_vector filtervec:(NSArray*)filtervec params:(NSString*)params type:(unsigned int)type
{
  NSMutableArray* args = [NSMutableArray arrayWithCapacity:0];
  [args addObject:[NSNumber numberWithUnsignedInt:type]];

  if ([params length] > 0) {
    for (NSString* p in [params componentsSeparatedByString : @","]) {
      unsigned int datatype = 0;
      unsigned int newvalue = 0;
      for (NSString* value in [p componentsSeparatedByString : @"|"]) {
        unsigned int newdatatype = 0;
        /*  */ if ([value hasPrefix:@"KeyCode::"]) {
          newdatatype = BRIDGE_DATATYPE_KEYCODE;
        } else if ([value hasPrefix:@"ModifierFlag::"]) {
          newdatatype = BRIDGE_DATATYPE_FLAGS;
        } else if ([value hasPrefix:@"ConsumerKeyCode::"]) {
          newdatatype = BRIDGE_DATATYPE_CONSUMERKEYCODE;
        } else if ([value hasPrefix:@"PointingButton::"]) {
          newdatatype = BRIDGE_DATATYPE_POINTINGBUTTON;
        } else if ([value hasPrefix:@"KeyboardType::"]) {
          newdatatype = BRIDGE_DATATYPE_KEYBOARDTYPE;
        } else if ([value hasPrefix:@"Option::"]) {
          newdatatype = BRIDGE_DATATYPE_OPTION;
        } else {
          @throw [NSException exceptionWithName : @"ConfigXMLParser" reason :[NSString stringWithFormat:@"unknown datatype: %@ (%@)", value, params] userInfo : nil];
        }

        if (datatype && datatype != newdatatype) {
          // Don't connect different data type. (Example: KeyCode::A | ModifierFlag::SHIFT_L)
          @throw [NSException exceptionWithName : @"ConfigXMLParser" reason :[NSString stringWithFormat:@"invalid connect(|): %@", params] userInfo : nil];
        }

        datatype = newdatatype;
        newvalue |= [keycode_ unsignedIntValue:value];
      }

      [args addObject:[NSNumber numberWithUnsignedInt:datatype]];
      [args addObject:[NSNumber numberWithUnsignedInt:newvalue]];
    }
  }

  [initialize_vector addObject:[NSNumber numberWithUnsignedInteger:[args count]]];
  [initialize_vector addObjectsFromArray:args];

  if ([filtervec count] > 0) {
    [initialize_vector addObjectsFromArray:filtervec];
  }
}

- (void) handle_autogen:(NSMutableArray*)initialize_vector filtervec:(NSArray*)filtervec autogen_text:(NSString*)autogen_text
{
  // ------------------------------------------------------------
  // preprocess
  //
  for (NSString* modifier in [NSArray arrayWithObjects : @"COMMAND", @"CONTROL", @"SHIFT", @"OPTION", nil]) {
    NSString* symbol = [NSString stringWithFormat:@"VK_%@", modifier];
    if ([autogen_text rangeOfString:symbol].location != NSNotFound) {
      [self handle_autogen:initialize_vector filtervec:filtervec
              autogen_text:[autogen_text stringByReplacingOccurrencesOfString:symbol withString:[NSString stringWithFormat:@"ModifierFlag::%@_L", modifier]]];
      [self handle_autogen:initialize_vector filtervec:filtervec
              autogen_text:[autogen_text stringByReplacingOccurrencesOfString:symbol withString:[NSString stringWithFormat:@"ModifierFlag::%@_R", modifier]]];
      return;
    }
  }

  if ([autogen_text rangeOfString:@"VK_MOD_CCOS_L"].location != NSNotFound) {
    autogen_text = [autogen_text stringByReplacingOccurrencesOfString:@"VK_MOD_CCOS_L" withString:@"ModifierFlag::COMMAND_L|ModifierFlag::CONTROL_L|ModifierFlag::OPTION_L|ModifierFlag::SHIFT_L"];
    [self handle_autogen:initialize_vector filtervec:filtervec autogen_text:autogen_text];
    return;
  }

  if ([autogen_text rangeOfString:@"VK_MOD_CCS_L"].location != NSNotFound) {
    autogen_text = [autogen_text stringByReplacingOccurrencesOfString:@"VK_MOD_CCS_L" withString:@"ModifierFlag::COMMAND_L|ModifierFlag::CONTROL_L|ModifierFlag::SHIFT_L"];
    [self handle_autogen:initialize_vector filtervec:filtervec autogen_text:autogen_text];
    return;
  }

  if ([autogen_text rangeOfString:@"VK_MOD_CCO_L"].location != NSNotFound) {
    autogen_text = [autogen_text stringByReplacingOccurrencesOfString:@"VK_MOD_CCO_L" withString:@"ModifierFlag::COMMAND_L|ModifierFlag::CONTROL_L|ModifierFlag::OPTION_L"];
    [self handle_autogen:initialize_vector filtervec:filtervec autogen_text:autogen_text];
    return;
  }

  if ([autogen_text rangeOfString:@"VK_MOD_ANY"].location != NSNotFound) {
    // to reduce combination, we ignore same modifier combination such as (COMMAND_L | COMMAND_R).
    NSMutableArray* combination = [self combination:[NSArray arrayWithObjects:@"VK_COMMAND", @"VK_CONTROL", @"ModifierFlag::FN", @"VK_OPTION", @"VK_SHIFT", nil]];
    for (NSMutableArray* a in combination) {
      [self handle_autogen:initialize_vector filtervec:filtervec
              autogen_text:[autogen_text stringByReplacingOccurrencesOfString:@"VK_MOD_ANY" withString:[[a arrayByAddingObject:@"ModifierFlag::NONE"] componentsJoinedByString:@"|"]]];
    }
    return;
  }

  for (NSString* keyname in [NSArray arrayWithObjects : @"HOME", @"END", @"PAGEUP", @"PAGEDOWN", @"FORWARD_DELETE", nil]) {
    if ([autogen_text rangeOfString:[NSString stringWithFormat:@"FROMKEYCODE_%@,ModifierFlag::", keyname]].location != NSNotFound) {
      [self handle_autogen:initialize_vector filtervec:filtervec
              autogen_text:[autogen_text stringByReplacingOccurrencesOfString:[NSString stringWithFormat:@"FROMKEYCODE_%@", keyname]
                                                                   withString:[NSString stringWithFormat:@"KeyCode::%@", keyname]]];
      [self handle_autogen:initialize_vector filtervec:filtervec
              autogen_text:[autogen_text stringByReplacingOccurrencesOfString:[NSString stringWithFormat:@"FROMKEYCODE_%@,", keyname]
                                                                   withString:[NSString stringWithFormat:@"KeyCode::%@,ModifierFlag::FN|", [self getextrakey:keyname]]]];
      return;
    }

    if ([autogen_text rangeOfString:[NSString stringWithFormat:@"FROMKEYCODE_%@", keyname]].location != NSNotFound) {
      [self handle_autogen:initialize_vector filtervec:filtervec
              autogen_text:[autogen_text stringByReplacingOccurrencesOfString:[NSString stringWithFormat:@"FROMKEYCODE_%@", keyname]
                                                                   withString:[NSString stringWithFormat:@"KeyCode::%@", keyname]]];
      [self handle_autogen:initialize_vector filtervec:filtervec
              autogen_text:[autogen_text stringByReplacingOccurrencesOfString:[NSString stringWithFormat:@"FROMKEYCODE_%@", keyname]
                                                                   withString:[NSString stringWithFormat:@"KeyCode::%@,ModifierFlag::FN", [self getextrakey:keyname]]]];
      return;
    }
  }

  if ([autogen_text rangeOfString:@"--KeyOverlaidModifierWithRepeat--"].location != NSNotFound) {
    [self handle_autogen:initialize_vector filtervec:filtervec
            autogen_text:[autogen_text stringByReplacingOccurrencesOfString:@"--KeyOverlaidModifierWithRepeat--"
                                                                 withString:@"--KeyOverlaidModifier--Option::KEYOVERLAIDMODIFIER_REPEAT,"]];
    return;
  }

  if ([autogen_text rangeOfString:@"SimultaneousKeyPresses::Option::RAW"].location != NSNotFound) {
    [self handle_autogen:initialize_vector filtervec:filtervec
            autogen_text:[autogen_text stringByReplacingOccurrencesOfString:@"SimultaneousKeyPresses::Option::RAW"
                                                                 withString:@"Option::SIMULTANEOUSKEYPRESSES_RAW"]];
    return;
  }

  // ------------------------------------------------------------
  // append to initialize_vector
  //

  if ([autogen_text hasPrefix:@"--ShowStatusMessage--"]) {
    NSString* params = [autogen_text substringFromIndex:[@"--ShowStatusMessage--" length]];
    params = [params stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]];

    const char* utf8string = [params UTF8String];
    size_t length = strlen(utf8string);
    [initialize_vector addObject:[NSNumber numberWithUnsignedLong:(length + 1)]];
    [initialize_vector addObject:[NSNumber numberWithUnsignedInt:BRIDGE_STATUSMESSAGE]];
    for (size_t i = 0; i < length; ++i) {
      [initialize_vector addObject:[NSNumber numberWithChar:utf8string[i]]];
    }

    // no need filtervec
    return;
  }

  if ([autogen_text hasPrefix:@"--SimultaneousKeyPresses--"]) {
    NSString* params = [autogen_text substringFromIndex:[@"--SimultaneousKeyPresses--" length]];

    NSString* newkeycode = [NSString stringWithFormat:@"VK_SIMULTANEOUSKEYPRESSES_%d", simultaneous_keycode_index_];
    [keycode_ append:@"KeyCode" name:newkeycode];
    ++simultaneous_keycode_index_;

    params = [NSString stringWithFormat:@"KeyCode::%@,%@", newkeycode, params];
    [self append_to_initialize_vector:initialize_vector filtervec:filtervec params:params type:BRIDGE_REMAPTYPE_SIMULTANEOUSKEYPRESSES];

    return;
  }

  static struct {
    NSString* symbol;
    unsigned int type;
  } info[] = {
    { @"--KeyToKey--",                       BRIDGE_REMAPTYPE_KEYTOKEY },
    { @"--KeyToConsumer--",                  BRIDGE_REMAPTYPE_KEYTOCONSUMER },
    { @"--KeyToPointingButton--",            BRIDGE_REMAPTYPE_KEYTOPOINTINGBUTTON },
    { @"--DoublePressModifier--",            BRIDGE_REMAPTYPE_DOUBLEPRESSMODIFIER },
    { @"--HoldingKeyToKey--",                BRIDGE_REMAPTYPE_HOLDINGKEYTOKEY },
    { @"--IgnoreMultipleSameKeyPress--",     BRIDGE_REMAPTYPE_IGNOREMULTIPLESAMEKEYPRESS },
    { @"--KeyOverlaidModifier--",            BRIDGE_REMAPTYPE_KEYOVERLAIDMODIFIER },
    { @"--ConsumerToConsumer--",             BRIDGE_REMAPTYPE_CONSUMERTOCONSUMER },
    { @"--ConsumerToKey--",                  BRIDGE_REMAPTYPE_CONSUMERTOKEY },
    { @"--PointingButtonToPointingButton--", BRIDGE_REMAPTYPE_POINTINGBUTTONTOPOINTINGBUTTON },
    { @"--PointingButtonToKey--",            BRIDGE_REMAPTYPE_POINTINGBUTTONTOKEY },
    { @"--PointingRelativeToScroll--",       BRIDGE_REMAPTYPE_POINTINGRELATIVETOSCROLL },
    { @"--DropKeyAfterRemap--",              BRIDGE_REMAPTYPE_DROPKEYAFTERREMAP },
    { @"--SetKeyboardType--",                BRIDGE_REMAPTYPE_SETKEYBOARDTYPE },
    { @"--DropPointingRelativeCursorMove--", BRIDGE_REMAPTYPE_DROPPOINTINGRELATIVECURSORMOVE },
    { NULL, 0 },
  };
  for (int i = 0; info[i].symbol; ++i) {
    if ([autogen_text hasPrefix:info[i].symbol]) {
      [self append_to_initialize_vector:initialize_vector filtervec:filtervec params:[autogen_text substringFromIndex:[info[i].symbol length]] type:info[i].type];
    }
  }
}

- (void) traverse_autogen:(NSMutableArray*)initialize_vector node:(NSXMLNode*)node name:(NSString*)name
{
  NSUInteger count = [node childCount];
  for (NSUInteger i = 0; i < count; ++i) {
    NSXMLNode* n = [node childAtIndex:i];
    if ([n kind] != NSXMLElementKind) continue;

    if ([[n name] isEqualToString:@"autogen"]) {
      NSMutableArray* filtervec = [self make_filtervec:n];

      if (! [name hasPrefix:@"passthrough_"]) {
        [filtervec addObject:[NSNumber numberWithUnsignedInt:2]];
        [filtervec addObject:[NSNumber numberWithUnsignedInt:BRIDGE_FILTERTYPE_CONFIG_NOT]];
        [filtervec addObject:[keycode_ numberValue:@"ConfigIndex::notsave_passthrough"]];
      }

      NSString* autogen_text = [n stringValue];
      autogen_text = [autogen_text stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]];

      // drop whitespaces for preprocessor. (for FROMKEYCODE_HOME, etc)
      // Note: preserve space when --ShowStatusMessage--.
      if (! [autogen_text hasPrefix:@"--ShowStatusMessage--"]) {
        autogen_text = [autogen_text stringByReplacingOccurrencesOfString:@" " withString:@""];
        autogen_text = [autogen_text stringByReplacingOccurrencesOfString:@"\r" withString:@""];
        autogen_text = [autogen_text stringByReplacingOccurrencesOfString:@"\t" withString:@""];
        autogen_text = [autogen_text stringByReplacingOccurrencesOfString:@"\n" withString:@""];
      }

      [self handle_autogen:initialize_vector filtervec:filtervec autogen_text:autogen_text];
    }

    [self traverse_autogen:initialize_vector node:n name:name];
  }
}

// ======================================================================
// initialize

- (void) append_to_keycode:(NSXMLElement*)element handle_notsave:(BOOL)handle_notsave
{
  NSArray* vk_config_formats = [NSArray arrayWithObjects:
                                @"VK_CONFIG_TOGGLE_%@",
                                @"VK_CONFIG_FORCE_ON_%@",
                                @"VK_CONFIG_FORCE_OFF_%@",
                                @"VK_CONFIG_SYNC_KEYDOWNUP_%@",
                                nil];

  for (NSXMLElement* e in [element elementsForName : @"identifier"]) {
    NSString* name = [KeyCode normalizeName:[e stringValue]];

    BOOL isnotsave = [name hasPrefix:@"notsave_"];
    if ((isnotsave && ! handle_notsave) ||
        (! isnotsave && handle_notsave)) {
      continue;
    }

    if ([e attributeForName:@"vk_config"]) {
      for (NSString* format in vk_config_formats) {
        [keycode_ append:@"KeyCode" name:[NSString stringWithFormat:format, name]];
      }
    }

    if (! [e attributeForName:@"essential"]) {
      [keycode_ append:@"ConfigIndex" name:name];
    }
  }

  for (NSXMLElement* e in [element elementsForName : @"list"]) {
    [self append_to_keycode:e handle_notsave:handle_notsave];
  }
  for (NSXMLElement* e in [element elementsForName : @"item"]) {
    [self append_to_keycode:e handle_notsave:handle_notsave];
  }
}

- (void) traverse_identifier:(NSXMLElement*)element
{
  for (NSXMLElement* e in [element elementsForName : @"identifier"]) {
    NSXMLNode* attr_essential = [e attributeForName:@"essential"];
    if (attr_essential) continue;

    NSMutableArray* initialize_vector = [NSMutableArray arrayWithCapacity:0];
    NSString* rawname = [e stringValue];
    NSString* name = [KeyCode normalizeName:rawname];

    if ([e attributeForName:@"vk_config"]) {
      [initialize_vector addObject:[NSNumber numberWithUnsignedInt:5]];
      [initialize_vector addObject:[NSNumber numberWithUnsignedInt:BRIDGE_VK_CONFIG]];
      [initialize_vector addObject:[keycode_ numberValue:[NSString stringWithFormat:@"KeyCode::VK_CONFIG_TOGGLE_%@", name]]];
      [initialize_vector addObject:[keycode_ numberValue:[NSString stringWithFormat:@"KeyCode::VK_CONFIG_FORCE_ON_%@", name]]];
      [initialize_vector addObject:[keycode_ numberValue:[NSString stringWithFormat:@"KeyCode::VK_CONFIG_FORCE_OFF_%@", name]]];
      [initialize_vector addObject:[keycode_ numberValue:[NSString stringWithFormat:@"KeyCode::VK_CONFIG_SYNC_KEYDOWNUP_%@", name]]];
    }

    [self traverse_autogen:initialize_vector node:[e parent] name:name];

    [initialize_vector insertObject:[NSNumber numberWithUnsignedInteger:[initialize_vector count]] atIndex:0];
    [initialize_vector insertObject:[NSNumber numberWithUnsignedInt:BRIDGE_REMAPCLASS_INITIALIZE_VECTOR_FORMAT_VERSION] atIndex:0];

    NSNumber* configindex = [keycode_ numberValue:[NSString stringWithFormat:@"ConfigIndex::%@", name]];
    [dict_initialize_vector_ setObject:initialize_vector forKey:configindex];
    [dict_config_name_ setObject:rawname forKey:configindex];
  }

  for (NSXMLElement* e in [element elementsForName : @"list"]) {
    [self traverse_identifier:e];
  }
  for (NSXMLElement* e in [element elementsForName : @"item"]) {
    [self traverse_identifier:e];
  }
}

// ======================================================================
- (NSString*) get_private_xml_path
{
  NSFileManager* filemanager = [NSFileManager defaultManager];
  NSArray* paths = NSSearchPathForDirectoriesInDomains(NSApplicationSupportDirectory, NSUserDomainMask, YES);
  NSString* path = [paths objectAtIndex:0];
  path = [path stringByAppendingPathComponent:@"KeyRemap4MacBook"];
  if (! [filemanager fileExistsAtPath:path]) {
    [filemanager createDirectoryAtPath:path withIntermediateDirectories:YES attributes:nil error:NULL];
  }
  if ([filemanager fileExistsAtPath:path]) {
    path = [path stringByAppendingPathComponent:@"private.xml"];
  }

  BOOL isDirectory;
  if ([filemanager fileExistsAtPath:path isDirectory:&isDirectory] && ! isDirectory) {
    return path;
  }

  return @"";
}


- (void) load {
  NSArray* paths = [NSArray arrayWithObjects:
                    [self get_private_xml_path],
                    @"/Library/org.pqrs/KeyRemap4MacBook/prefpane/checkbox.xml",
                    nil];

  NSMutableDictionary* xmldocdict = [[[NSMutableDictionary alloc] initWithCapacity:0] autorelease];

  @try {
    for (NSString* xmlpath in paths) {
      if ([xmlpath length] == 0) continue;

      NSURL* url = [NSURL fileURLWithPath:xmlpath];
      NSXMLDocument* xmldocument = [[[NSXMLDocument alloc] initWithContentsOfURL:url options:0 error:NULL] autorelease];
      if (xmldocument) {
        [xmldocdict setObject:xmldocument forKey:xmlpath];
      }

      [self append_to_keycode:[xmldocument rootElement] handle_notsave:YES];
      [self append_to_keycode:[xmldocument rootElement] handle_notsave:NO];
    }

    for (NSString* xmlpath in paths) {
      NSXMLDocument* xmldocument = [xmldocdict objectForKey:xmlpath];
      if (xmldocument) {
        [self traverse_identifier:[xmldocument rootElement]];
      }
    }

    initialized_ = YES;

  } @catch (NSException* exception) {
    NSLog(@"[ERROR] KeyRemap4MacBook_server %@ %@", [exception name], [exception reason]);
  }
}

- (id) init
{
  [super init];

  simultaneous_keycode_index_ = 0;
  dict_initialize_vector_ = [[NSMutableDictionary alloc] initWithCapacity:0];
  dict_config_name_       = [[NSMutableDictionary alloc] initWithCapacity:0];
  keycode_ = [KeyCode new];
  initialized_ = NO;

  [self load];

  return self;
}

- (void) dealloc
{
  if (dict_initialize_vector_) {
    [dict_initialize_vector_ release];
  }
  if (dict_config_name_) {
    [dict_config_name_ release];
  }
  if (keycode_) {
    [keycode_ release];
  }

  [super dealloc];
}

// ------------------------------------------------------------
- (NSUInteger) count
{
  NSUInteger v = 0;
  @synchronized(self) {
    if (initialized_) {
      v = [dict_initialize_vector_ count];
    }
  }
  return v;
}

- (NSArray*) initialize_vector:(unsigned int)configindex
{
  NSArray* a = nil;
  @synchronized(self) {
    if (initialized_) {
      a = [dict_initialize_vector_ objectForKey:[NSNumber numberWithUnsignedInt:configindex]];
    }
  }
  return a;
}

- (NSUInteger) initialize_vector_size:(unsigned int)configindex
{
  NSUInteger v = 0;
  @synchronized(self) {
    NSArray* a = [self initialize_vector:configindex];
    if (a) {
      v = [a count];
    }
  }
  return v;
}

- (unsigned int) keycode:(NSString*)name
{
  unsigned int v = 0;
  @synchronized(self) {
    if (initialized_) {
      v = [keycode_ unsignedIntValue:name];
    }
  }
  return v;
}

- (NSString*) configname:(unsigned int)configindex
{
  NSString* name = nil;
  @synchronized(self) {
    if (initialized_) {
      name = [dict_config_name_ objectForKey:[NSNumber numberWithUnsignedInt:configindex]];
    }
  }
  return name;
}

@end
