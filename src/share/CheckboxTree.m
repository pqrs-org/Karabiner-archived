#import "CheckboxTree.h"

static NSInteger checkboxItemId_ = 0;

@interface CheckboxItem ()

@property(readwrite) NSNumber* id;
@property(copy, readwrite) NSString* name;
@property(copy, readwrite) NSString* style;
@property(copy, readwrite) NSString* identifier;

@end

@implementation CheckboxItem

- (instancetype)initWithName:(const char*)name style:(const char*)style identifier:(const char*)identifier {
  self = [super init];

  if (self) {
    ++checkboxItemId_;
    _id = @(checkboxItemId_);
    _name = name ? @(name) : @"";
    _style = style ? @(style) : @"";
    _identifier = identifier ? @(identifier) : @"";
  }

  return self;
}

- (BOOL)needsShowCheckbox {
  if ([self.identifier length] == 0 ||
      [self.identifier hasPrefix:@"notsave."]) {
    return NO;
  }
  return YES;
}

#pragma mark - NSObject

- (id)replacementObjectForPortCoder:(NSPortCoder*)encoder {
  if ([encoder isBycopy]) return self;
  return [super replacementObjectForPortCoder:encoder];
}

#pragma mark - NSCoding

- (instancetype)initWithCoder:(NSCoder*)decoder {
  self = [super init];

  if (self) {
    _id = [decoder decodeObjectForKey:@"id"];
    _name = [decoder decodeObjectForKey:@"name"];
    _style = [decoder decodeObjectForKey:@"style"];
    _identifier = [decoder decodeObjectForKey:@"identifier"];
  }

  return self;
}

- (void)encodeWithCoder:(NSCoder*)encoder {
  [encoder encodeObject:self.id forKey:@"id"];
  [encoder encodeObject:self.name forKey:@"name"];
  [encoder encodeObject:self.style forKey:@"style"];
  [encoder encodeObject:self.identifier forKey:@"identifier"];
}

#pragma mark - NSCoping

- (id)copyWithZone:(NSZone*)zone {
  CheckboxItem* obj = [[[self class] allocWithZone:zone] init];
  if (obj) {
    obj.id = [self.id copyWithZone:zone];
    obj.name = [self.name copyWithZone:zone];
    obj.style = [self.style copyWithZone:zone];
    obj.identifier = [self.identifier copyWithZone:zone];
  }
  return obj;
}

@end

@interface CheckboxTree ()

@property(readwrite) CheckboxItem* node;
@property(copy, readwrite) NSArray* children;

@end

@implementation CheckboxTree

- (instancetype)initWithItem:(CheckboxItem*)node
                    children:(NSArray*)children {
  self = [super init];

  if (self) {
    _node = node;
    _children = children;
  }

  return self;
}

#pragma mark - NSObject

- (id)replacementObjectForPortCoder:(NSPortCoder*)encoder {
  if ([encoder isBycopy]) return self;
  return [super replacementObjectForPortCoder:encoder];
}

#pragma mark - NSCoding

- (instancetype)initWithCoder:(NSCoder*)decoder {
  self = [super init];

  if (self) {
    _node = [decoder decodeObjectForKey:@"node"];
    _children = [decoder decodeObjectForKey:@"children"];
  }

  return self;
}

- (void)encodeWithCoder:(NSCoder*)encoder {
  [encoder encodeObject:self.node forKey:@"node"];
  [encoder encodeObject:self.children forKey:@"children"];
}

#pragma mark - NSCoping

- (id)copyWithZone:(NSZone*)zone {
  CheckboxTree* obj = [[[self class] allocWithZone:zone] init];
  if (obj) {
    obj.node = [self.node copyWithZone:zone];
    obj.children = [self.children copyWithZone:zone];
  }
  return obj;
}

@end
