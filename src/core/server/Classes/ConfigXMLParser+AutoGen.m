#import "ConfigXMLParser.h"
#import "bridge.h"

@implementation ConfigXMLParser (AutoGen)

// ======================================================================
// filter

- (void) append_to_filter:(NSMutableArray*)filters node:(NSXMLNode*)node prefix:(NSString*)prefix filtertype:(unsigned int)filtertype
{
  NSArray* a = [[node stringValue] componentsSeparatedByString:@","];

  NSUInteger count = [a count] + 1;
  [filters addObject:[NSNumber numberWithUnsignedInteger:count]];
  [filters addObject:[NSNumber numberWithUnsignedInt:filtertype]];

  for (NSString* name in a) {
    // support '|' for <modifier_only>.
    // For example: <modifier_only>ModifierFlag::COMMAND_L|ModifierFlag::CONTROL_L, ModifierFlag::COMMAND_L|ModifierFlag::OPTION_L</modifier_only>
    unsigned int value = 0;
    for (NSString* v in [name componentsSeparatedByString : @"|"]) {
      value |= [keycode_ unsignedIntValue:[NSString stringWithFormat:@"%@%@", prefix, [KeyCode normalizeName:v]]];
    }
    [filters addObject:[NSNumber numberWithUnsignedInt:value]];
  }
}

- (NSMutableArray*) make_filtervec:(NSXMLNode*)parent_node_of_autogen
{
  NSMutableArray* filters = [NSMutableArray arrayWithCapacity:0];

  for (;;) {
    if (! parent_node_of_autogen) break;

    NSUInteger count = [parent_node_of_autogen childCount];
    for (NSUInteger i = 0; i < count; ++i) {
      NSXMLNode* n = [parent_node_of_autogen childAtIndex:i];
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

      } else if ([n_name isEqualToString:@"modifier_not"]) {
        [self append_to_filter:filters node:n prefix:@"" filtertype:BRIDGE_FILTERTYPE_MODIFIER_NOT];
      } else if ([n_name isEqualToString:@"modifier_only"]) {
        [self append_to_filter:filters node:n prefix:@"" filtertype:BRIDGE_FILTERTYPE_MODIFIER_ONLY];

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

    if ([[parent_node_of_autogen name] isEqualToString:@"item"]) break;

    parent_node_of_autogen = [parent_node_of_autogen parent];
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
          @throw [NSException exceptionWithName : @"<autogen> error" reason :[NSString stringWithFormat:@"unknown datatype: %@ (%@)", value, params] userInfo : nil];
        }

        if (datatype && datatype != newdatatype) {
          // Don't connect different data type. (Example: KeyCode::A | ModifierFlag::SHIFT_L)
          @throw [NSException exceptionWithName : @"<autogen> error" reason :[NSString stringWithFormat:@"invalid connect(|): %@", params] userInfo : nil];
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
    { @"--ForceNumLockOn--",                 BRIDGE_REMAPTYPE_FORCENUMLOCKON },
    { @"--DropPointingRelativeCursorMove--", BRIDGE_REMAPTYPE_DROPPOINTINGRELATIVECURSORMOVE },
    { NULL, 0 },
  };
  for (int i = 0; info[i].symbol; ++i) {
    if ([autogen_text hasPrefix:info[i].symbol]) {
      [self append_to_initialize_vector:initialize_vector filtervec:filtervec params:[autogen_text substringFromIndex:[info[i].symbol length]] type:info[i].type];
      return;
    }
  }

  @throw [NSException exceptionWithName : @"<autogen> error" reason :[NSString stringWithFormat:@"unknown parameters: %@", autogen_text] userInfo : nil];
}

- (void) traverse_autogen:(NSMutableArray*)initialize_vector node:(NSXMLNode*)node name:(NSString*)name
{
  NSUInteger count = [node childCount];
  NSMutableArray* filtervec = nil;

  for (NSUInteger i = 0; i < count; ++i) {
    NSXMLNode* n = [node childAtIndex:i];
    if ([n kind] != NSXMLElementKind) continue;

    if ([[n name] isEqualToString:@"autogen"]) {
      if (! filtervec) {
        filtervec = [self make_filtervec:[n parent]];

        if (! [name hasPrefix:@"passthrough_"]) {
          [filtervec addObject:[NSNumber numberWithUnsignedInt:2]];
          [filtervec addObject:[NSNumber numberWithUnsignedInt:BRIDGE_FILTERTYPE_CONFIG_NOT]];
          [filtervec addObject:[keycode_ numberValue:@"ConfigIndex::notsave_passthrough"]];
        }
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
  NSUInteger count = [element childCount];
  for (NSUInteger i = 0; i < count; ++i) {
    NSXMLElement* e = [self castToNSXMLElement:[element childAtIndex:i]];
    if (! e) continue;

    if (! [[e name] isEqualToString:@"identifier"]) {
      [self append_to_keycode:e handle_notsave:handle_notsave];

    } else {
      NSString* name = [KeyCode normalizeName:[e stringValue]];

      BOOL isnotsave = [name hasPrefix:@"notsave_"];
      if ((isnotsave && ! handle_notsave) ||
          (! isnotsave && handle_notsave)) {
        continue;
      }

      if ([e attributeForName:@"vk_config"]) {
        [keycode_ append:@"KeyCode" name:[NSString stringWithFormat:@"VK_CONFIG_TOGGLE_%@", name]];
        [keycode_ append:@"KeyCode" name:[NSString stringWithFormat:@"VK_CONFIG_FORCE_ON_%@", name]];
        [keycode_ append:@"KeyCode" name:[NSString stringWithFormat:@"VK_CONFIG_FORCE_OFF_%@", name]];
        [keycode_ append:@"KeyCode" name:[NSString stringWithFormat:@"VK_CONFIG_SYNC_KEYDOWNUP_%@", name]];
      }

      if (! [e attributeForName:@"essential"]) {
        [keycode_ append:@"ConfigIndex" name:name];
      }
    }
  }
}

- (void) traverse_identifier:(NSXMLElement*)element
{
  NSUInteger count = [element childCount];
  for (NSUInteger i = 0; i < count; ++i) {
    NSXMLElement* e = [self castToNSXMLElement:[element childAtIndex:i]];
    if (! e) continue;

    if (! [[e name] isEqualToString:@"identifier"]) {
      [self traverse_identifier:e];

    } else {
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
  }
}

// ======================================================================
// check identifier
- (void) check_conflict_identifier:(NSMutableDictionary*)identifier_dictionary element:(NSXMLElement*)element
{
  NSUInteger count = [element childCount];
  for (NSUInteger i = 0; i < count; ++i) {
    NSXMLElement* e = [self castToNSXMLElement:[element childAtIndex:i]];
    if (! e) continue;

    if (! [[e name] isEqualToString:@"identifier"]) {
      [self check_conflict_identifier:identifier_dictionary element:e];

    } else {
      NSString* rawname = [e stringValue];
      NSString* name = [KeyCode normalizeName:rawname];

      if ([identifier_dictionary objectForKey:name]) {
        @throw [NSException
              exceptionWithName: @"A conflict of <identifier>."
              reason:[NSString stringWithFormat:@"\"%@\" already exists. Rename it.\n\n%@", rawname, [e XMLString]]
              userInfo: nil];
      }
      [identifier_dictionary setObject:name forKey:name];
    }
  }
}


// ======================================================================
- (BOOL) reload_autogen {
  BOOL retval = NO;

  simultaneous_keycode_index_ = 0;

  if (dict_initialize_vector_) {
    [dict_initialize_vector_ release];
  }
  dict_initialize_vector_ = [NSMutableDictionary new];

  if (dict_config_name_) {
    [dict_config_name_ release];
  }
  dict_config_name_ = [NSMutableDictionary new];

  if (keycode_) {
    [keycode_ release];
  }
  keycode_ = [KeyCode new];

  // ------------------------------------------------------------
  NSMutableDictionary* xmldocdict = [[NSMutableDictionary new] autorelease];

  // --------------------
  // load XML
  //
  NSArray* paths = [self get_xml_paths];
  for (NSArray* pathinfo in paths) {
    NSString* xmlpath = [pathinfo objectAtIndex:0];
    NSNumber* xmltype = [pathinfo objectAtIndex:1];

    @try {
      if ([xmltype intValue] != CONFIGXMLPARSER_XML_TYPE_CHECKBOX) continue;
      if ([xmlpath length] == 0) continue;

      NSURL* url = [NSURL fileURLWithPath:xmlpath];
      NSError* error = nil;
      NSXMLDocument* xmldocument = [[[NSXMLDocument alloc] initWithContentsOfURL:url options:0 error:&error] autorelease];
      if (! xmldocument) {
        @throw [NSException exceptionWithName :[NSString stringWithFormat:@"%@ is invalid", xmlpath] reason :[error localizedDescription] userInfo : nil];
      }

      [xmldocdict setObject:xmldocument forKey:xmlpath];

    } @catch (NSException* exception) {
      [self setErrorMessage:exception xmlpath:xmlpath];
    }
  }

  // --------------------
  // check conflict of <identifier>.
  //   1st loop: OWNER_SYSTEM
  //   2nd loop: OWNER_USER
  //
  int loopcount = 0;
  NSMutableDictionary* identifier_dictionary = [[NSMutableDictionary new] autorelease];
  for (loopcount = 0; loopcount < 2; ++loopcount) {
    for (NSArray* pathinfo in paths) {
      NSString* xmlpath = [pathinfo objectAtIndex:0];
      NSNumber* owner   = [pathinfo objectAtIndex:2];

      @try {
        NSXMLDocument* xmldocument = [xmldocdict objectForKey:xmlpath];
        if (! xmldocument) continue;

        if (loopcount == 0 && [owner intValue] != CONFIGXMLPARSER_XML_OWNER_SYSTEM) continue;
        if (loopcount == 1 && [owner intValue] != CONFIGXMLPARSER_XML_OWNER_USER) continue;

        [self check_conflict_identifier:identifier_dictionary element:[xmldocument rootElement]];

      } @catch (NSException* exception) {
        [self setErrorMessage:exception xmlpath:xmlpath];
        [xmldocdict removeObjectForKey:xmlpath];
      }
    }
  }

  // --------------------
  // append_to_keycode.
  //   1st loop: <identifier>notsave.*</identifier>
  //   2nd loop: other <identifier>
  //
  for (loopcount = 0; loopcount < 2; ++loopcount) {
    for (NSArray* pathinfo in paths) {
      NSString* xmlpath = [pathinfo objectAtIndex:0];

      @try {
        NSXMLDocument* xmldocument = [xmldocdict objectForKey:xmlpath];
        if (! xmldocument) continue;

        if (loopcount == 0) {
          [self append_to_keycode:[xmldocument rootElement] handle_notsave:YES];
        } else if (loopcount == 1) {
          [self append_to_keycode:[xmldocument rootElement] handle_notsave:NO];
        }

      } @catch (NSException* exception) {
        [self setErrorMessage:exception xmlpath:xmlpath];
      }
    }
  }

  // --------------------
  // parse <autogen>
  //
  for (NSArray* pathinfo in paths) {
    NSString* xmlpath = [pathinfo objectAtIndex:0];

    @try {
      NSXMLDocument* xmldocument = [xmldocdict objectForKey:xmlpath];
      if (! xmldocument) continue;

      [self traverse_identifier:[xmldocument rootElement]];

      // Set retval to YES if only one XML file is loaded successfully.
      // Unless we do it, all setting becomes disabled by one error.
      // (== If private.xml is invalid, system wide checkbox.xml is not loaded in kext.)
      retval = YES;

    } @catch (NSException* exception) {
      [self setErrorMessage:exception xmlpath:xmlpath];
    }
  }

  return retval;
}

@end
